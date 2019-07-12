module Styles = {
  open Css;
  let overlay =
    style([
      position(fixed),
      top(px(0)),
      left(px(0)),
      width(pct(100.)),
      height(pct(100.)),
      background(rgba(24, 24, 24, 0.85)),
      display(`flex),
      justifyContent(center),
      alignItems(center),
    ]);

  let boxRoot =
    style([
      position(fixed),
      top(pct(50.)),
      left(pct(50.)),
      display(`flex),
      transform(translate(pct(-50.), pct(-50.))),
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
  let submitButton =
    style([position(relative), minWidth(px(200)), marginLeft(px(15))]);
  let boxActions =
    style([
      display(`flex),
      flexDirection(row),
      alignItems(center),
      justifyContent(center),
      marginTop(px(20)),
    ]);
};

module ModalForm = {
  open Formality;

  type field =
    | Link
    | Zip;

  type state = {
    link: string,
    zip: string,
  };

  type message = string;
  type submissionError = unit;

  module LinkField = {
    let update = (state, value) => {...state, link: value};

    let validator = {
      field: Link,
      strategy: Strategy.OnFirstSuccessOrFirstBlur,
      dependents: None,
      validate: ({link}) =>
        switch (link) {
        | "" => Error("Email is required")
        | _ => Ok(Valid)
        },
    };
  };

  module ZipField = {
    let update = (state, value) => {...state, zip: value};
  };

  let validators = [LinkField.validator];
};

module ModalFormHook = Formality.Make(ModalForm);

let initialState = ModalForm.{link: "", zip: ""};

[@react.component]
let make = (~isVisible: bool, ~onClose) => {
  let form =
    ModalFormHook.useForm(
      ~initialState,
      ~onSubmit=(state, form) => {
        Files.add(~url=state.link, ~extract=state.zip === "on") |> ignore;
        form.notifyOnSuccess(None);
        onClose();

        ();
      },
    );

  isVisible ?
    <>
      <div className=Styles.overlay onClick={_event => onClose()} />
      <div className=Styles.boxRoot>
        <div className=Styles.box>
          <h2 className=Styles.boxTitle> {React.string("New download")} </h2>
          <form
            className=Styles.boxForm
            onSubmit=form.submit->Formality.Dom.preventDefault>
            <Input
              value={form.state.link}
              _type=`Text
              placeholder="Link"
              disabled={form.submitting}
              inputClassName=Styles.input
              onChange={
                event =>
                  form.change(
                    Link,
                    ModalForm.LinkField.update(
                      form.state,
                      event->ReactEvent.Form.target##value,
                    ),
                  )
              }
              onBlur={_ => form.blur(Link)}
              error={
                switch (Link->(form.result)) {
                | Some(Error(message)) => message->React.string
                | Some(Ok(Valid))
                | Some(Ok(NoValue))
                | None => React.null
                }
              }
            />
            <div className=Styles.checkboxContainer>
              <input
                id="zip"
                type_="checkbox"
                checked={form.state.zip == "on"}
                onChange={
                  event =>
                    form.change(
                      Zip,
                      ModalForm.ZipField.update(
                        form.state,
                        event->ReactEvent.Form.target##value,
                      ),
                    )
                }
              />
              <label htmlFor="zip">
                {React.string("Unzip after download")}
              </label>
            </div>
            <div className=Styles.boxActions>
              <Button
                _type=`Button
                backgroundColor="#fff"
                color="#333"
                onClick={_ => onClose()}
                content={React.string("Cancel")}
              />
              <Button
                _type=`Submit
                disabled={form.submitting}
                className=Styles.submitButton
                content=
                  <>
                    {
                      form.submitting ?
                        <Loader
                          size=Loader.Tiny
                          isActive=true
                          color="#fff"
                          className=Styles.loader
                        /> :
                        React.null
                    }
                    {
                      (form.submitting ? "Submitting..." : "Add")
                      |> React.string
                    }
                  </>
              />
            </div>
          </form>
        </div>
      </div>
    </> :
    React.null;
};