namespace glm {
	struct vec4 {
		float x;
		float y;
		float z;
		float w;
	};
}

%extend glm::vec4 {
  %proxycode %{
    UnityEngine.Vector4 toVector4() {
      return new UnityEngine.Vector4(Native.bve_csPINVOKE.glm_vec4_x_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec4_y_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec4_z_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec4_w_get(swigCPtr));
    }
  %}
}
