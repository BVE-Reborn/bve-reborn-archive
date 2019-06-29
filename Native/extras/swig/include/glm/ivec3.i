namespace glm {
  struct ivec3 {
    ivec3(int x, int y, int z);
    int x;
    int y;
    int z;
  };
}

%extend glm::ivec3 {
  int* get_self() {
    return (int*) $self;
  }
  %proxycode %{
    public unsafe UnityEngine.Vector3Int toVector2() {
      int* ptr = (int*) global::Native.raw.intp.get_raw(get_self());
      return new UnityEngine.Vector3Int(ptr[0], ptr[1], ptr[2]);
    }
  %}
}
