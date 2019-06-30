%{
  #include <glm/glm.hpp>
%}

namespace glm {
	struct vec4 {
		float x;
		float y;
		float z;
		float w;
	};
}

%extend glm::vec4 {
  float* get_self() {
    return (float*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Vector2 toVector4() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Vector4(ptr[0], ptr[1], ptr[2], ptr[3]);
    }
    public unsafe UnityEngine.Color toColor() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Color(ptr[0], ptr[1], ptr[2], ptr[3]);
    }
  %}
}
