oml_define_svector(value_type, SIZE);

#define container_type oml_svector(value_type, SIZE)
#define iterator_type oml_svector_iterator(value_type, SIZE)

#define test_svector_init(this) oml_svector_init(this)
