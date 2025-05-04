#ifndef MIRAGE_REFLECT
#define MIRAGE_REFLECT


namespace mirage 
{
	#define MIRAGE_CLASS(x) \
	 constexpr  static bool __CLASS__##x##_ = true; \

	#define MIRAGE_STRUCT(x) \
	 constexpr  static bool __STRUCT__##x##_ = true;\

	#define MIRAGE_ENUM(x) \
	constexpr static bool __STRUCT__##x##_ = true;\

	#define MIRAGE_ATTRIBUTES(x, ...) \
	constexpr  static const char* __PROPERTY__##x## = #__VA_ARGS__#;
	//sstd::make_tuple(__VA_ARGS__)\

	#define MIRAGE_FUNC(x, ...)\ // TO DO
}

#endif 