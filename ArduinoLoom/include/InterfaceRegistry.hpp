/**
 * loom/InterfaceRegistry.hpp
 *
 * A central place where interfaces can self-register
 * Reduces the need to update the manager files for new interfaces
 * 
 * @author Alex Schlieck
 * @date 2023-04-19
*/

// TODO: Rename file to registry

#include <vector>
#include "Interface.hpp"

namespace loom
{
	/// @brief Abstract of class that will create new interfaces
	class BaseInterfaceFactory
	{
	public:
		/// @brief the type of interface that will be created by this factory
		String classId;

		/// @brief Create and register an interface
		/// @return pointer to new interface
		virtual Interface* createInterface() = 0;
	};



	/// @brief Store the factories used to make the interfaces self-registering
	/// uses a singleton
	class InterfaceRegistry
	{
	public:

		/// @brief Take and store an interface factory for later use
		/// @param factory pointer to the factory
		static void addFactory(BaseInterfaceFactory* factory)
		{
			instance().factories.push_back(factory);
		}

		static bool contains(String classId)
		{
			for (auto factory : instance().factories)
			{
				if (factory->classId == classId)
				{
					return true;
				}
			}
			return false;
		}

		/// @brief create and return a new interface of the specified type
		/// @param classId the type of interface to create
		/// @return pointer to the newly created interface
		static Interface* getInterface(String classId)
		{
			Interface* result = nullptr;
			// find the correct factory by id
			for (auto factory : instance().factories)
			{
				if (factory->classId == classId)
				{
					// create a new interface
					result = factory->createInterface();
				}
			}
			return result;
		}

	private:
		InterfaceRegistry() = default;
		~InterfaceRegistry() = default;

		/// @brief get access to the single instance of the manager
		/// @return reference to the single instance
		static InterfaceRegistry& instance()
		{
			static InterfaceRegistry instance;
			return instance;
		}

		/// @brief Stores all the registered factories
		std::vector<BaseInterfaceFactory*> factories;
	};


	/// @brief Create and register a factory for a specific type of Interface
	/// @tparam InterfaceClass the type of interface to create with the factory
	template <class InterfaceClass>
	class InterfaceRegistration : public BaseInterfaceFactory
	{
	public:
		/// @brief Constructor, registers factory on creation
		/// @param id type of interface to create with the class
		InterfaceRegistration(String classId)
		{
			BaseInterfaceFactory::classId = classId;
			//InterfaceRegistry::instance().addFactory(this);
			InterfaceRegistry::addFactory(this);
		}

		/// @brief Create a new instance of the interface
		/// @return pointer to the new interface instance
		virtual Interface* createInterface()
		{
			return new InterfaceClass;
		}
	};

} // namespace loom
