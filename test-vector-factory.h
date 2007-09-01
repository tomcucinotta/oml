oml_define_vector(value_type);

#define container_type oml_vector(value_type)
#define iterator_type oml_vector_iterator(value_type)

#define test_vector_init(this) oml_vector_init((this), SIZE)
