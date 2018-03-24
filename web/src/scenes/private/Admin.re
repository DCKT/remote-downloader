let component = "Admin" |> ReasonReact.statelessComponent;

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> (ReasonReact.stringToElement("Admin")) </h1> </div>,
};