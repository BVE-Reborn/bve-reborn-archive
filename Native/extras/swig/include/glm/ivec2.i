namespace glm {
  struct ivec2 {
    ivec2(int x, int y);
    int x;
    int y;
  };
}

%extend glm::ivec2 {
  int* get_self() {
    return (int*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Vector2Int toVector2() {
      int* ptr = (int*) global::Native.raw.intp.get_raw(get_self());
      return new UnityEngine.Vector2Int(ptr[0], ptr[1]);
    }
  %}
}
