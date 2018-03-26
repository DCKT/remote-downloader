let component = "NotFound" |> ReasonReact.statelessComponent;

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <h1> (ReasonReact.stringToElement("404 page not found")) </h1>
    </div>,
};