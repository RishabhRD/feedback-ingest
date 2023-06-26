# Feedback Ingest

This is a multi-source, multi-tenant feedback ingestion application.

The goal of the codebase is to be as extensible as possible, so that:
- Its easy to support new data sources
- Its easy to switch low-level implementations for scalability purposes

## Building

### Prerequisites

- conan (version >= 2.0)
- cmake
- make
- clang

### Steps

Commands are intended to be executed from root of project:
```bash
./update_package # Install the dependencies
./build # Build the binaries
```

## Running tests

```bash
bin/tests/feeback_ingest_test
```

## Running program


Feedback ingest gets data source information from loader files.
Create a directory name `loader_files`.

- loader_files/tenant.txt

This file describes the different tenant information.
File contains multiple lines of structure:
```
<tenant_id> <email> <organisation>
```

A sample file looks like:

```
1 enterpret@enterpret.com Enterpret
2 rishabhdwivedi17@gmail.com Local
```

**NOTE:** tenant_id used in this file is just for loading purposes and have no
connection with tenant_id used in application.

- loader_files/discourse.txt

This describes the discourse data source and tenants fetching from that data
source.

The file contains multiple lines of format:
```
<source name> <topic id> <number of subscribing tenants> <tenants>....
```

A sample file looks like:
```
Discourse 111143 2 1 2
Discourse 31963 2 1 2
```

- loader_files/filepp.txt

This describes the filepp data source and tenants fetching from that data
source.

The file contains multiple lines of format:
```
<source name> <file path> <number of subscribing tenants> <tenants>....
```

A sample file looks like:
```
filepp input_files/filepp_conversation_1.json 2 1 2
filepp input_files/filepp_conversation_2.json 1 1
```

A sample filepp json file looks like:
```json
[
  {
    "app_version": "4.18.0",
    "conversation": [
      {
        "User": "Can you help me with adding post to story button"
      },
      {
        "User": "Now, I can't see it. Its become hard to use."
      },
      {
        "Support": "You don't need help sir!"
      },
      {
        "Support": "It's not a bug, it's a feature!!"
      }
    ]
  },
  {
    "app_version": "4.19.0",
    "conversation": [
      {
        "User": "Hey! Add to story button is not even visible now"
      },
      {
        "User": "Why is that?"
      },
      {
        "Support": "This is a new feature. Hope you enjoy it."
      }
    ]
  }
]
```

- loader_files/notifyu.txt

This describes the notifyu data source and tenants fetching from that data
source.

The file contains multiple lines of format:
```
<source name> <exposed rest api route> <subscribing tenant>
```

By design one source can only have one tenant subscriber. Wrong file data
would be treated as undefined behavior.

A sample file looks like:
```
notifyu /notifyu_path_1 2
notifyu /notifyu_path_2 1
```

For accessing exposed API, curl can be used:
```
curl -X POST -H "Content-Type: application/json" -d @sample.json http://localhost:4000/notifyu_path_1
```

Where sample.json looks like:
```json
{
  "secret_token": "12345678",
  "review_stream": [
    {
      "app_version": "4.18.9",
      "country": "India",
      "rating": 4.3,
      "likes": 28,
      "review": "Nice App, needs a dark theme"
    },
    {
      "app_version": "4.18.8",
      "country": "Bangladesh",
      "rating": 4.8,
      "likes": 1,
      "review": "Good app"
    }
  ]
}
```


## High level Design decisions

- Software is written with having batch processing in mind. This decision was
  made based on nature of data (that is user feedback) as delay is affordable
  here.

## Low level Design decisions

- Event + Data oriented design is chosen over object oriented system.
- Third party coroutines and executors library is used as first class citizen.
- Values and objects are differentiated as much as possible.
