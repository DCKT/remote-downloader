let component = "Modal" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let overlay =
    style([
      position(fixed),
      top(px(0)),
      left(px(0)),
      width(pct(100.)),
      height(pct(100.)),
      background(rgba(24, 24, 24, 0.6)),
      display(`flex),
      justifyContent(center),
      alignItems(center),
    ]);
  let box =
    style([
      position(relative),
      background(hex("fff")),
      padding(px(40)),
      boxShadow(~x=px(1), ~y=px(3), ~blur=px(4), rgba(24, 24, 24, 0.2)),
      width(pct(100.)),
      maxWidth(px(450)),
      minHeight(px(200)),
      margin(auto),
    ]);
  let boxTitle =
    style([
      fontSize(px(24)),
      color(hex("333")),
      textAlign(center),
      borderBottom(px(1), solid, hex("69798F")),
      paddingBottom(px(20)),
    ]);
  let boxForm = style([marginTop(px(20))]);
  let input = style([width(pct(100.))]);
  let checkboxContainer = style([marginTop(px(15))]);
  let loader =
    style([
      position(absolute),
      left(px(15)),
      top(pct(50.)),
      marginTop(px(-10)),
    ]);
  let submitButton = style([position(relative), minWidth(px(200))]);
  let boxActions =
    style([
      display(`flex),
      flexDirection(row),
      alignItems(center),
      justifyContent(center),
    ]);
};

let make = (~isVisible: bool, ~onClose, _children) => {
  ...component,
  render: _self =>
    isVisible ?
      <Fragment>
        <div className=Styles.overlay onClick=(_event => onClose()) />
        <div className=Styles.box>
          <h2 className=Styles.boxTitle>
            (ReasonReact.stringToElement("New download"))
          </h2>
          <ModalForm.FormContainer
            initialState={link: "", zip: ""}
            onSubmit=(
              (state, notify) => {
                Firebase.addFile(~url=state.link, ~extract=state.zip == "on")
                |> ignore;
                notify.onSuccess();
                onClose();
                ();
              }
            )>
            ...(
                 form =>
                   <form
                     className=Styles.boxForm
                     onSubmit=(form.submit |> Formality.Dom.preventDefault)>
                     <Input
                       value=form.state.link
                       _type=`Url
                       label=(ReasonReact.stringToElement("Download url"))
                       placeholder="http://"
                       inputClassName=Styles.input
                       disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                       onChange=(
                         event =>
                           event
                           |> Formality.Dom.toValueOnChange
                           |> form.change(ModalForm.Form.Link)
                       )
                       onBlur=(
                         event =>
                           event
                           |> Formality.Dom.toValueOnBlur
                           |> form.change(ModalForm.Form.Link)
                       )
                       error=(
                         switch (ModalForm.Form.Link |> form.results) {
                         | Some(Invalid(message)) =>
                           message |> ReasonReact.stringToElement
                         | Some(Valid)
                         | None => ReasonReact.nullElement
                         }
                       )
                     />
                     <div className=Styles.checkboxContainer>
                       <input
                         id="zip"
                         _type="checkbox"
                         checked=(
                           form.state.zip == "on" |> Js.Boolean.to_js_boolean
                         )
                         onChange=(
                           _event => {
                             let newValue = form.state.zip == "on" ? "" : "on";
                             newValue |> form.change(ModalForm.Form.Zip);
                           }
                         )
                       />
                       <label htmlFor="zip">
                         (ReasonReact.stringToElement("Unzip after download"))
                       </label>
                     </div>
                     <div className=Styles.boxActions>
                       <Button
                         _type=`Button
                         backgroundColor="#fff"
                         color="#333"
                         onClick=(_ => onClose())
                         content=(ReasonReact.stringToElement("Cancel"))
                       />
                       <Button
                         _type=`Submit
                         disabled=(form.submitting |> Js.Boolean.to_js_boolean)
                         className=Styles.submitButton
                         content={
                           <Fragment>
                             (
                               form.submitting ?
                                 <Loader
                                   size=Loader.Tiny
                                   isActive=true
                                   color="#fff"
                                   className=Styles.loader
                                 /> :
                                 ReasonReact.nullElement
                             )
                             (
                               (form.submitting ? "Submitting..." : "Add")
                               |> ReasonReact.stringToElement
                             )
                           </Fragment>
                         }
                       />
                     </div>
                   </form>
               )
          </ModalForm.FormContainer>
        </div>
      </Fragment> :
      ReasonReact.nullElement,
};