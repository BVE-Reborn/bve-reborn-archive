#[repr(C)]
pub struct Test {
	pub f: f32
}


#[no_mangle]
pub extern fn rust_doubler_struct2(f: Test) -> Test {
	Test { f: {f.f * 2.0} }
}
