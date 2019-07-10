[@bs.deriving abstract]
type diskInfos = {
  available: int,
  free: int,
  total: int,
};

[@bs.module "diskusage"]
external checkDiskUsage: (string, BsNode.callback(diskInfos)) => unit =
  "check";