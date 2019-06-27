namespace glm {
  struct vec2 {
    vec2(float x, float y);
    float x;
    float y;
  };
}


%extend glm::vec2 {
  %proxycode %{
    UnityEngine.Vector2 toVector2() {
      return new UnityEngine.Vector2(Native.bve_csPINVOKE.glm_vec2_x_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec2_y_get(swigCPtr));
    }
  %}
}
