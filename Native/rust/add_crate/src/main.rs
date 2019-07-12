use clap::{Arg, App};
use std::process::{Command, exit};
use std::env::current_dir;
use std::fs::File;
use std::io;
use std::io::{Read, ErrorKind, Write};
use toml::Value;
use toml::map::{Keys, Map};
use toml::ser::Serializer;
use serde::ser::Serialize;
use std::error::Error;

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

fn add_to_cbindgen(name: &str) -> io::Result<()> {
    let mut buf = current_dir()?.to_path_buf();
    buf.push("cbindgen-meta");
    buf.push("Cargo.toml");

    let mut cargo_toml = File::open(buf.as_path())?;
    let mut contents = String::new();
    cargo_toml.read_to_string(&mut contents)?;

    let mut parsed = match contents.parse::<Value>() {
        Ok(value) => value,
        Err(err) => return Err(std::io::Error::new(ErrorKind::InvalidData, err.to_string()))
    };

    let mut parent = parsed["dependencies"].as_table_mut().unwrap();
    let mut addition = Map::new();
    addition.insert(String::from("path"), Value::String(format!("../{}", name)));
    parent.insert(name.to_owned(), Value::Table(addition));

    println!("{:#?}", parsed);

    let mut contents = String::new();
    parsed.serialize(&mut Serializer::pretty(&mut contents)).expect("Error reconstructing string");

    let mut cargo_toml = File::create(buf.as_path())?;
    match cargo_toml.write(contents.as_bytes()) {
        Ok(bytes) => {
            println!("wrote {} bytes", bytes);
        }
        Err(err) => {
            eprintln!("Error writing {}: {}", buf.to_str().unwrap(), err.description())
        }
    };

    Ok(())
}

fn main() {
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
    run_cargo(name);
    add_to_cbindgen(name);
}
