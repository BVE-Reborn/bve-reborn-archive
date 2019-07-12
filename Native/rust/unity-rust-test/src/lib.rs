use unity_rust_test_2::Test;

#[no_mangle]
pub extern fn rust_doubler(f: f32) -> f32 {
	f * 2.0
}

#[no_mangle]
pub extern fn rust_doubler_struct(f: Test) -> Test {
	Test { f: {f.f * 2.0} }
}
