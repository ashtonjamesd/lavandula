# Lavender ORM

! This is currently not supported and still under development !

<br/>

Lavender is the ORM layer for Lavandula. It provides a mechanism for running migrations on your database.

The following lavender schema represents a todo model with three fields. 

```
model Todo {
    id    Integer @id
    title String  @max(255)
    done  Boolean @default(false)
}
```

This model equates to the following sql:

```sql
CREATE TABLE Todo (
    id INTEGER PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    done BOOLEAN NOT NULL DEFAULT FALSE
);
```

## Lavender Schema

When using Lavender, the convention is to have a 'schema.lavender' in the same directory as your 'lavandula.yml'.


## Running Migrations

When you have made a change to your schema, you must run a migration for the changes to take effect. To create a new database migration, run the following command.

```
lavu migrate
```

This will inspect your 'schema.lavender' and create a new migration in the migrations folder. It is not recommended that you edit any of the migration files. The migrations are automatically applied to your database when ran.