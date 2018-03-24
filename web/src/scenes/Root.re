let component = ReasonReact.statelessComponent("Root");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <Router>
        ...(
             (~currentRoute) =>
               switch (currentRoute) {
               | Login => <Login />
               | Admin => <Admin />
               | NotFound => <NotFound />
               }
           )
      </Router>
    </div>,
};