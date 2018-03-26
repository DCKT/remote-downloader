let component = "Login" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let root =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      background(hex("0288D1")),
      height(pct(100.)),
    ]);
  let box =
    style([
      background(hex("fff")),
      width(px(500)),
      padding(px(40)),
      boxShadow(~x=px(0), ~y=px(3), ~blur=px(9), rgba(24, 24, 24, 0.3)),
    ]);
  let form = style([marginTop(px(40))]);
  let title =
    style([fontSize(px(28)), fontWeight(700), textAlign(center)]);
  let input =
    style([display(block), marginBottom(px(15)), textAlign(center)]);
  let submit =
    style([
      display(block),
      margin4(~top=px(25), ~left=auto, ~right=auto, ~bottom=px(0)),
      background(hex("0288D1")),
      fontSize(px(20)),
      color(hex("fff")),
      padding2(~v=px(10), ~h=px(30)),
      borderRadius(px(2)),
      border(px(1), solid, hex("0288D1")),
      cursor(`pointer),
      boxShadow(~x=px(0), ~y=px(2), ~blur=px(4), rgba(24, 24, 24, 0.1)),
    ]);
};

let make = (~isLogged: bool, ~connectUser, _children) => {
  ...component,
  didMount: _self => {
    if (isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Admin));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div className=Styles.root>
      <div className=Styles.box>
        <h1 className=Styles.title>
          (ReasonReact.stringToElement("Welcome"))
        </h1>
        <LoginForm.FormContainer
          initialState={email: "", password: ""}
          onSubmit=(
            (state, notify) =>
              /* Submit form and either notify.onSuccess / notify.onFailure */
              ()
          )>
          ...(
               form =>
                 <form
                   className=Styles.form
                   onSubmit=(form.submit |> Formality.Dom.preventDefault)>
                   <Input
                     value=form.state.email
                     _type=`Email
                     placeholder="Email"
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                     containerClassName=Styles.input
                     onChange=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnChange
                         |> form.change(LoginForm.Form.Email)
                     )
                     onBlur=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnBlur
                         |> form.blur(LoginForm.Form.Email)
                     )
                     error=(
                       switch (LoginForm.Form.Email |> form.results) {
                       | Some(Invalid(message)) =>
                         message |> ReasonReact.stringToElement
                       | Some(Valid)
                       | None => ReasonReact.nullElement
                       }
                     )
                   />
                   <Input
                     value=form.state.password
                     _type=`Password
                     placeholder="**********"
                     containerClassName=Styles.input
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                     onChange=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnChange
                         |> form.change(LoginForm.Form.Password)
                     )
                     onBlur=(
                       event =>
                         event
                         |> Formality.Dom.toValueOnBlur
                         |> form.blur(LoginForm.Form.Password)
                     )
                     error=(
                       switch (LoginForm.Form.Password |> form.results) {
                       | Some(Invalid(message)) =>
                         <div> (message |> ReasonReact.stringToElement) </div>
                       | Some(Valid)
                       | None => ReasonReact.nullElement
                       }
                     )
                   />
                   <button
                     className=Styles.submit
                     disabled=(form.submitting |> Js.Boolean.to_js_boolean)>
                     (
                       (form.submitting ? "Submitting..." : "Login")
                       |> ReasonReact.stringToElement
                     )
                   </button>
                 </form>
             )
        </LoginForm.FormContainer>
      </div>
    </div>,
  /* <form>
       <button onClick=connectUser>
         (ReasonReact.stringToElement("Connect me"))
       </button>
     </form> */
};