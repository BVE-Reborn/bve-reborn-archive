if ([System.Environment]::GetEnvironmentVariable("VCPKG_ROOT", "User") -eq "") {
    echo "Cannot find environment variable VCPKG_ROOT."
    return 1
}

$vcpkg_root = [System.Environment]::GetEnvironmentVariable("VCPKG_ROOT", "User")

$vcpkg_ports_dir = $vcpkg_root + "\ports\"

cp "-Recurse" "extern\ports\nova-renderer" "$vcpkg_root\ports\"

vcpkg install --recurse `
	"abseil:x64-windows" `
	"concurrentqueue:x64-windows" `
	"cppfs:x64-windows" `
	"doctest:x64-windows" `
	"fmt:x64-windows" `
	"glm:x64-windows" `
	"glslang:x64-windows" `
	"gtest:x64-windows" `
	"mapbox-variant:x64-windows" `
	"minitrace:x64-windows" `
	"miniz:x64-windows" `
	"ms-gsl:x64-windows" `
	"nlohmann-json:x64-windows" `
	"nova-renderer:x64-windows" `
	"rapidxml-ns:x64-windows" `
	"spirv-cross:x64-windows" `
	"spirv-tools:x64-windows" `
	"spirv-tools:x64-windows" `
	"vulkan-memory-allocator:x64-windows"
	"vulkan:x64-windows" `
