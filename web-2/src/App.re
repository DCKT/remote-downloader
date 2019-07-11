open ReasonReactNavigation;

module RouterConfig = {
  type route =
    | Login
    | Admin
    | NotFound;

  let routeFromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | [] => Login
    | ["admin"] => Admin
    | ["404"]
    | _ => NotFound
    };
  let routeToUrl = (route: route) =>
    switch (route) {
    | Login => "/"
    | Admin => "/admin"
    | NotFound => "/404"
    };
};

module Router = CreateRouter(RouterConfig);

module PrivateOnly = {
  [@react.component]
  let make = (~authenticated: bool, ~children) => {
    React.useEffect1(
      () => {
        if (!authenticated) {
          Router.replace(Login);
        };
        None;
      },
      [|authenticated|],
    );

    children;
  };
};

[@react.component]
let make = () => {
  let (appSync, setAppSync) = React.useState(() => false);
  let (authenticated, setAuthenticated) = React.useState(() => false);

  React.useEffect1(
    () => {
      Firebase.Authentication.onAuthStateChanged(
        Firebase.authInstance,
        user => {
          setAppSync(_ => true);
          setAuthenticated(_ =>
            user->Js.Nullable.toOption->Belt.Option.isSome
          );
        },
      )
      |> ignore;

      None;
    },
    [||],
  );

  appSync ?
    <Router.Container>
      ...{
           (~currentRoute) =>
             switch (currentRoute) {
             | RouterConfig.Login => <AnonymousScreen />
             | RouterConfig.Admin =>
               <PrivateOnly authenticated> <AdminScreen /> </PrivateOnly>
             | _ => <NotFound />
             }
         }
    </Router.Container> :
    <LoadingScreen />;
};

let default = make;