namespace glm {
	struct vec3 {
		float x;
		float y;
		float z;
	};
}

%extend glm::vec3 {
  %proxycode %{
    UnityEngine.Vector3 toVector3() {
      return new UnityEngine.Vector3(Native.bve_csPINVOKE.glm_vec3_x_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec3_y_get(swigCPtr),
                                     Native.bve_csPINVOKE.glm_vec3_z_get(swigCPtr));
    }
  %}
}
