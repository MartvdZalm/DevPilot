# Repository Layer

## Overview
The repository layer provides a clean abstraction over database operations. Each repository handles CRUD operations for a specific database table/model and encapsulates all SQL logic.

## Architecture

### Repository Pattern
- **Interface** (`IAppRepository`) - Defines the contract for data access operations
- **Implementation** (`AppRepository`) - Concrete implementation with actual SQL queries

### Key Components

#### IAppRepository (Interface)
Defines all data access methods:
- `findById()` - Retrieve single record by ID
- `findAll()` - Retrieve all records
- `save()` - Insert or update (handles both automatically)
- `deleteById()` - Remove a record
- `mapFromRecord()` - Convert database query to model object

#### AppRepository (Implementation)
Implements the interface with actual database operations using Qt's SQL module.

**Key Features:**
- Uses `std::optional<T>` for nullable return values
- Automatic insert vs update detection in `save()`
- Comprehensive error logging

## How It Works

### Save Operation
```cpp
std::optional<App> save(const App& app)
{
    return app.getId() > 0 ? update(app) : insert(app);
}
```
Automatically determines whether to insert (new record) or update (existing record) based on ID.

### Error Handling
All database operations log errors and return appropriate values:
- `std::optional<T>` for single records (nullopt on failure)
- Empty lists for collection queries
- `false` for boolean operations

## Creating a New Repository

1. **Create the interface** (`IUserRepository.h`)
```cpp
   class IUserRepository
   {
     public:
       virtual ~IUserRepository() = default;
       virtual std::optional<User> findById(int id) = 0;
       virtual QList<User> findAll() = 0;
       virtual std::optional<User> save(const User& user) = 0;
       virtual bool deleteById(int id) = 0;
   };
```

2. **Implement the repository** (`UserRepository.h` and `.cpp`)
   - Inherit from the interface
   - Inject `QSqlDatabase&` via constructor
   - Implement all CRUD methods
   - Add `insert()` and `update()` as private helpers

3. **Register in RepositoryProvider**
   - Add getter method for the new repository

4. **Register in main.cpp**
   - Initialize and make available for dependency injection

5. **Use via dependency injection**
   - Inject into UI components or services as needed

## Best Practices

- **Use interfaces** for all repositories to enable mocking in tests
- **Return `std::optional`** for operations that may fail or return no results
- **Log all errors** with descriptive messages including relevant IDs
- **Keep SQL in repositories** - Never expose SQL queries to other layers
- **Use prepared statements** - Always bind values to prevent SQL injection

## Benefits

- **Testability** - Easy to mock repositories for unit testing
- **Maintainability** - SQL logic in one place
- **Type Safety** - Compile-time checking of data access operations
- **Consistency** - All repositories follow the same pattern
- **Separation of Concerns** - Database logic isolated from business logic
