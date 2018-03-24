module Config = {
  type route =
    | Login
    | Admin
    | NotFound;
  let routeFromUrl = (url: ReasonReact.Router.url) => {
    Js.log(url.path);
    switch (url.path) {
    | ["login"] => Login
    | [] => Admin
    | _ => NotFound
    };
  };
  let routeToUrl = (route: route) =>
    switch (route) {
    | Login => "/login"
    | Admin => "/"
    | NotFound => "404"
    };
};

type action =
  | ChangeRoute(Config.route);

type state = {currentRoute: Config.route};

let component = "Router" |> ReasonReact.reducerComponent;

let make = children => {
  ...component,
  initialState: () => {
    currentRoute:
      Config.routeFromUrl(ReasonReact.Router.dangerouslyGetInitialUrl()),
  },
  reducer: (action, _state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update({currentRoute: route})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeRoute(Config.routeFromUrl(url)))
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self => children(~currentRoute=self.state.currentRoute),
};