%{
  #include <glm/glm.hpp>
%}

namespace glm {
	struct u8vec1 {
		uint8_t x;
		uint8_t r;
	};
}

%extend glm::u8vec1 {
  uint8_t* get_self() {
    return (uint8_t*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Color32 toColor32() {
      byte* ptr = (byte*) global::Native.raw.bytep.get_raw(get_self());
      return new UnityEngine.Color32(ptr[0], 0, 0, 0);
    }
  %}
}
