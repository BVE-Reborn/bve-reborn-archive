namespace glm {
  struct vec2 {
    vec2(float x, float y);
    float x;
    float y;
  };
}

%extend glm::vec2 {
  float* get_self() {
    return (float*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Vector2 toVector2() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Vector2(ptr[0], ptr[1]);
    }
    public unsafe UnityEngine.Color toColor() {
      float* ptr = (float*) global::Native.raw.floatp.get_raw(get_self());
      return new UnityEngine.Color(ptr[0], ptr[1], 0.0f, 0.0f);
    }
  %}
}
