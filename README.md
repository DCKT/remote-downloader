# REMOTE DOWNLOADER

Download files from anywhere 😎

## Setup
There is a `README` for each project, take a look !

## How it works

This project uses Firebase to avoid database, hosting and some painful server management.

This project is split into 2 projects :

* **server** : listen Firebase Database update and trigger actionn
* **client** : web interface to add and remove database elements

### Lifecycle

1.  The client create a new **file ref** with some params

```
{
    "id": <idVar>,
    "url": <urlVar>,
    "extract": <extractVar>,
    "progress": 0,
    "size": 0,
    "status": "waiting"
}
```

2.  The server listen the `child_added` event and will trigger the download if the `status` of the added file is `waiting`.

3.  On the `request` stream event :

    * Update the `size` data (total size of the file in octet).
    * Change the `status` to `pending`.
    * Add the `filename`

4.  On each `data` stream event, we update the download `progress` (in octet).
    Note : Obviously, there is a `throttle` bound to the event to avoid data consumption.

5.  On `end` stream event :

    * Change the `status` to `completed`

6.  When the download is finished, we update the `filepath`.
