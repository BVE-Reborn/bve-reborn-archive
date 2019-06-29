namespace glm {
	struct vec3 {
		float x;
		float y;
		float z;
	};
}

%extend glm::vec3 {
  float* get_self() {
    return (float*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Vector2 toVector3() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Vector3(ptr[0], ptr[1], ptr[2]);
    }
    public unsafe UnityEngine.Color toColor() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Color(ptr[0], ptr[1], ptr[2], 0.0f);
    }
  %}
}
