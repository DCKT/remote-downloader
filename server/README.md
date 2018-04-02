# Server

## Setup

### Firebase admin credentials

This project use two different configuration file, one for `development` and another for `production`.
After you've downloaded the config files, put them in `src/firebase/data` and name them like this :

* `dev.json` for development
* `prod.json` for production

### Firebase database structure

```
{
    "disk": {
        "available": int,
        "free": int,
        "total": int
    },
    "files": {
        "fileKey": {
            "extract": bool,
            "folder": string
            "id": string,
            "progress": int,
            "status": "waiting" | "pending" | "completed" | "error"
        }
    },
    "folders: {
        "list": string
    }
}
```

### Download folder path

By default, the download folder path will `download` at the root of the project. If you want to edit this, create a `.env` file with an environment variable :

```
DOWNLOAD_FOLDER_PATH=../any/where/you/have/right
```

**Note**: If the folder doesn't exist, it will be created ! So don't forget to check permissions.

### Install dependencies

The project was written with Node.js LTS, it hasn't been tested on other versions.
To install dependencies run this command :

```
npm i
```

### Run the program

Just run `npm start`

If you want to run it as a daemon, I suggest you to use [pm2](http://pm2.keymetrics.io/)

```
pm2 start npm --name "Your APP Name" -- start
```
