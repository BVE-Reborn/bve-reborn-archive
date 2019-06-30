%{
  #include <glm/glm.hpp>
%}

namespace glm {
	struct u8vec2 {
		uint8_t x;
		uint8_t y;
		uint8_t r;
		uint8_t g;
	};
}

%extend glm::u8vec2 {
  uint8_t* get_self() {
    return (uint8_t*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Color32 toColor32() {
      byte* ptr = (byte*) global::Native.raw.bytep.get_raw(get_self());
      return new UnityEngine.Color32(ptr[0], ptr[1], 0, 0);
    }
  %}
}
