#include <IsNdArray/array_registry.hpp>
#include <IsNdArray/common.hpp>

#include <memory>
#include <string>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::shared_ptr;
		using std::vector;
		using std::string;

        /* ------------------- ArrayGroup ------------------- */
        // private static
        ArrayGroup::Registry_t& ArrayGroup::get_registry()
		{
			static Registry_t registry_;
			return registry_;
		}
        

        // public static
        void ArrayGroup::add_group(const string& array_class,
			                       const string& group_name)
		{
			Registry_t& registry = get_registry();
			registry[array_class] = group_name;
		}


        // public static
        string ArrayGroup::get_group(const string& array_class)
		{
			init_cpu();
			Registry_t& registry = get_registry();
			try
			{
				return registry.at(array_class);
			}
			catch (std::out_of_range&)
			{
				vector<string> keys;
				for (auto& kv : registry)
				{
					keys.push_back(kv.first);
				}
				NBLA_ERROR(error_code::unclassified, "'%s' cannot be found in [%s].",
					    array_class.c_str(), string_join(keys, ", ").c_str());
			}
		}

        /* ------------------- ArrayCreator ------------------- */
        // private static
        ArrayCreator::Registry_t& ArrayCreator::get_registry()
		{
			static Registry_t registry_;
			return registry_;
		}


        static 
        void check_registry_contains_class_or_throw(
                                    const ArrayCreator::Registry_t& registry,
				                    const string& array_class)
		{
			// TODO: assert registry.count(ctx.array_class) == 1
			if (registry.count(array_class) != 0)
			{
				return;
			}

			vector<string> keys;
			for (auto& kv : registry)
			{
				keys.push_back(kv.first);
			}
			NBLA_ERROR(error_code::unclassified, "'%s' cannot be found in [%s].",
				       array_class.c_str(), string_join(keys, ", ").c_str());
		}


        // public static
        Array* ArrayCreator::create(const Size_t size, dtypes dtype, const Context& ctx)
		{
			init_cpu();
			Registry_t& registry = get_registry();
			check_registry_contains_class_or_throw(registry, ctx.array_class);
			return registry[ctx.array_class].first(size, dtype, ctx); // Creator
		}


        // public static
        Context ArrayCreator::filter_context(const Context& ctx)
		{
			init_cpu();
			Registry_t& registry = get_registry();
			check_registry_contains_class_or_throw(registry, ctx.array_class);
			return registry[ctx.array_class].second(ctx); // FilterContext
		}


        // public static
        void ArrayCreator::add_creator(const string& name, Creator creator, FilterContext filter_context)
		{
			Registry_t& registry = get_registry();

			// TODO: assert registry.count(name) == 0
			registry[name] = { creator, filter_context };
		}

        /* ------------------- ArraySynchronizer ------------------- */
        // private static
        ArraySynchronizer::Registry_t& ArraySynchronizer::get_registry()
		{
			static Registry_t registry_;
			return registry_;
		}


        // public static
        void ArraySynchronizer::synchronize(const string& src_class, Array* src_array,
                                            const string& dst_class, Array* dst_array,
                                            const int async_flags) 
        {
            init_cpu();
            Registry_t &registry = get_registry();
            pair<string, string> key{ src_class, dst_class };
            NBLA_CHECK(registry.count(key) == 1, error_code::unclassified,
                        [registry, key]() {
                        std::ostringstream ss;
                        ss << key.first << "-" << key.second << " is not in (";
                        for (auto &kv : registry) {
                            ss << kv.first.first << "-" << kv.first.second << ", ";
                        }
                        ss << ").";
                        return ss.str();
                        }().c_str()); // TODO: Display key list that has been registered.

            registry[key](src_array, dst_array, async_flags);
        }


        // public static
        void ArraySynchronizer::add_synchronizer(const string& src_class,
			                                     const string& dst_class,
			                                     Synchronizer synchronizer)
		{
			Registry_t& registry = get_registry();
			pair<string, string> key{ src_class, dst_class };
			// TODO: assert registry.count(key) == 0
			registry[key] = synchronizer;
		}


        // public static
        void synchronizer_default(Array* src, Array* dst, const int async_flags)
        {
            // Wait for an previous asynchronous memcpy
            src->wait_event(dst->context(), async_flags);

            if (dst->have_event())
            {
                NBLA_ERROR(error_code::target_specific_async,
                           "Duplicated memcpy to the same destination array");
            }

            dst->copy_from(src);
        }
    }
}