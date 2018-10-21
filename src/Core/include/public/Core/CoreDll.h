#ifdef EXPORT_CORE_API
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif