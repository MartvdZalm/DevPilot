# Repositories
This folder contains the data access layer of the application. Each repository provides access to a specific model (e.g., `Project`, `App`) and abstracts away database logic.

## Adding a new repository
1. Create an interface (e.g. `IUserRepository`) and make the repository class implement it for better abstraction and testability.
2. Create a new class (e.g. `UserRepository`) that implements the interface.
3. Register the new repository in `RepositoryProvider`.
4. Add the newly made repository in `main.cpp` to make it available via dependency injection. 
5. Use dependency injection to access the repository in UI components or services.

Repositories follow a consistent pattern for maintainability and testability.
