#[deny(rust_2018_idioms)]

use clap::{Arg, App};
use std::process::{Command, exit};
use std::env::current_dir;
use std::fs::File;
use std::io;
use std::io::{ErrorKind, Write};
use toml::Value;
use toml::map::Map;
use toml::ser::Serializer;
use serde::ser::Serialize;
use std::error::Error;
use std::path::{PathBuf, Path};
use std::ops::Div;

struct PathWrapper {
    path: PathBuf,
}

impl PathWrapper {
    pub fn new() -> PathWrapper {
        PathWrapper {
            path: PathBuf::new()
        }
    }

    pub fn to_path(&self) -> &Path {
        &*self.path.as_path()
    }
}

impl From<&Path> for PathWrapper {
    fn from(path: &Path) -> Self {
        PathWrapper {
            path: path.to_path_buf(),
        }
    }
}

impl From<PathBuf> for PathWrapper {
    fn from(path_buf: PathBuf) -> Self {
        PathWrapper {
            path: path_buf,
        }
    }
}

impl Div<&str> for PathWrapper {
    type Output = PathWrapper;

    fn div(mut self, rhs: &str) -> Self::Output {
        self.path.push(rhs);
        self
    }
}

impl Div<&str> for &PathWrapper {
    type Output = PathWrapper;

    fn div(self, rhs: &str) -> Self::Output {
        let mut tmp = self.path.clone();
        tmp.push(rhs);
        PathWrapper {
            path: tmp
        }
    }
}

fn run_cargo(name: &str) {
    let mut command =
        Command::new("cargo");

    command.args(&["new", "--lib", name]);

    match command.output() {
        Ok(output) => {
            if output.status.success() {
                println!("Added cargo crate successfully");
            }
            else {
                eprintln!("Error running cargo.\n\nSTDOUT:\n{}\n\nSTDERR:\n{}",
                          String::from_utf8(output.stdout).unwrap(),
                          String::from_utf8(output.stderr).unwrap());
                exit(1);
            }
        }
        Err(error) => {
            eprintln!("Error running cargo: {}", error.to_string());
            eprintln!("Failed to run cargo. Is cargo in your path?");
            exit(1);
        }
    }
}

fn add_to_cbindgen_meta(name: &str) -> io::Result<()> {
    let buf = PathWrapper::from(current_dir()?) / "cbindgen-meta" / "Cargo.toml";

    let contents = std::fs::read_to_string(buf.to_path()).unwrap();

    let mut parsed = match contents.parse::<Value>() {
        Ok(value) =>
            value,
        Err(err) =>
            return Err(io::Error::new(ErrorKind::InvalidData, err.to_string()))
    };

    let parent = parsed["dependencies"].as_table_mut().unwrap();
    let mut addition = Map::new();
    addition.insert(String::from("path"), Value::String(format!("../{}", name)));
    parent.insert(name.to_owned(), Value::Table(addition));

    let mut contents = String::new();
    parsed.serialize(&mut Serializer::pretty(&mut contents)).expect("Error reconstructing string");

    let mut cargo_toml = File::create(buf.to_path())?;
    match cargo_toml.write(contents.as_bytes()) {
        Ok(bytes) => {
            println!("wrote {} bytes to {:?}", bytes, buf.to_path());
        }
        Err(err) => {
            eprintln!("Error writing {}: {}", buf.path.to_str().unwrap(), err.description());
            exit(1);
        }
    };

    Ok(())
}

fn add_to_cbindgen(name: &str) -> io::Result<()> {
    let buf = PathWrapper::from(current_dir()?) / "cbindgen.toml";

    let contents = std::fs::read_to_string(buf.to_path()).unwrap();
    let mut parsed = match contents.parse::<Value>() {
        Ok(value) =>
            value,
        Err(err) =>
            return Err(io::Error::new(ErrorKind::InvalidData, err.to_string()))
    };

    let parent = parsed["parse"].as_table_mut().unwrap();
    let member = parent["extra_bindings"].as_array_mut().unwrap();
    member.push(Value::String(name.to_owned()));

    let mut contents = String::new();
    parsed.serialize(&mut Serializer::pretty(&mut contents)).expect("Error reconstructing string");

    let mut cargo_toml = File::create(buf.to_path())?;
    match cargo_toml.write(contents.as_bytes()) {
        Ok(bytes) => {
            println!("wrote {} bytes to {:?}", bytes, buf.to_path());
        }
        Err(err) => {
            eprintln!("Error writing {}: {}", buf.path.to_str().unwrap(), err.description());
            exit(1);
        }
    };

    Ok(())
}

fn main() -> io::Result<()> {
    let matches =
        App::new("bve-reborn add crate utility")
            .version("0.1")
            .author("Connor Fitzgerald <connorwadefitzgerald@gmail.com>")
            .about("Adds a crate to bve-reborn's rust components")
            .arg(Arg::with_name("NAME")
                .help("Name of crate to create")
                .required(true)
                .index(1))
            .get_matches();

    let name = matches.value_of("NAME").unwrap();

    let result = PathWrapper::from(current_dir().unwrap()) / name;
    let path = result.to_path();

    if path.exists() {
        eprintln!("Crate {} already exists", name);
        exit(0);
    }

    run_cargo(name);
    add_to_cbindgen(name)?;
    add_to_cbindgen_meta(name)?;
    Ok(())
}
