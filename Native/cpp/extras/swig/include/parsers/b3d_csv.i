%rename(parse_helper_add_error) bve::parsers::errors::add_error;
%rename(parse_model_b3d) bve::parsers::b3d_csv_object::parse_b3d;
%rename(parse_model_csv) bve::parsers::b3d_csv_object::parse_csv;

%{
  #include <parsers/b3d_csv.hpp>
  #include <core/image/loader.hpp>
  using namespace bve;
  using namespace bve::parsers;
%}

%include <util/datatypes.hpp>
%include <parsers/errors.hpp>
%include <parsers/dependencies.hpp>
%include <parsers/b3d_csv.hpp>
