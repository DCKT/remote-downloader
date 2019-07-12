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

module AdminOnly = {
  [@react.component]
  let make = (~authenticated: bool, ~children) => {
    React.useEffect1(
      () => {
        if (!authenticated) {
          Router.navigate(Login);
        };
        None;
      },
      [|authenticated|],
    );

    children;
  };
};

module AnonymousOnly = {
  [@react.component]
  let make = (~authenticated: bool, ~children) => {
    React.useEffect1(
      () => {
        if (authenticated) {
          Router.replace(Admin);
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
          setAuthenticated(_ =>
            user->Js.Nullable.toOption->Belt.Option.isSome
          );
          setAppSync(_ => true);
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
             | RouterConfig.Login =>
               <AnonymousOnly authenticated>
                 <LoginScreen
                   onLoginSuccess=(
                     () => {
                       Router.navigate(Admin);
                       setAuthenticated(_ => true);
                     }
                   )
                 />
               </AnonymousOnly>
             | RouterConfig.Admin =>
               <AdminOnly authenticated> <AdminScreen /> </AdminOnly>
             | _ => <NotFound />
             }
         }
    </Router.Container> :
    <LoadingScreen />;
};

let default = make;