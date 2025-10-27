# Seeders

## Overview
The seeding system provides a way to populate the database with default data when the application starts.
It uses a modular pattern where each type of data has its own seeder class.

## Architecture

### ISeeder Interface
Base interface that all seeders implement with three methods:
- `shouldSeed()` - Checks if seeding is needed
- `seed()` - Performs the actual seeding
- `getName()` - Returns the seeder name for logging

### Concrete Seeders
Individual seeder implementations (like `ProcessTemplateSeeder`) that seed specific data types.

## How It Works
1. Register seeders with the using `addSeeder()`
2. Call `runSeeders()` to execute all seeders
3. Each seeder checks if it should run via `shouldSeed()`
4. If needed, the seeder executes its `seed()` method
5. Progress is logged throughout the process

## Creating a New Seeder

1. Inherit from `ISeeder`
2. Implement `shouldSeed()`, `seed()`, and `getName()`
3. Register with the `Seeder` during app initialization
