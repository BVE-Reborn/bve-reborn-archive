%rename(data_impl) bve::core::image::Loader::data() const noexcept;

%include <core/image/loader.hpp>

%extend bve::core::image::Loader {
  %proxycode %{
    public unsafe global::UnityEngine.Color[] data() {
      var dim = dimensions();
      var raw_ptr = global::Native.raw.bytep.get_raw(data_impl());
      var len = dim.x * dim.y;
      var array = new global::UnityEngine.Color[len];
      for (int i = 0; i < len; ++i) {
        array[i] = new global::UnityEngine.Color(raw_ptr[4 * i + 0], 
                                                 raw_ptr[4 * i + 1], 
                                                 raw_ptr[4 * i + 2], 
                                                 raw_ptr[4 * i + 3]);
      }
      return array;
    }
  %}
}
