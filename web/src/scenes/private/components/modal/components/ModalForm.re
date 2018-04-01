module Form = {
  type field =
    | Link
    | Zip;
  type value = string;
  type state = {
    link: string,
    zip: string,
  };
  type message = string;
  let get = (field, state) =>
    switch (field) {
    | Link => state.link
    | Zip => state.zip
    };
  let update = ((field, value), state) =>
    switch (field, value) {
    | (Link, value) => {...state, link: value}
    | (Zip, value) => {...state, zip: value}
    };
  let valueEmpty = value => value === "";
  let strategy = Formality.Strategy.OnFirstSuccessOrFirstBlur;
  module Validators =
    Formality.MakeValidators(
      {
        type t = field;
      },
    );
  type validators =
    Validators.t(Formality.validator(field, value, state, message));
  let validators =
    Formality.(
      Validators.empty
      |> Validators.add(
           Link,
           {
             strategy,
             dependents: None,
             validate: (value, _state) =>
               switch (value) {
               | "" => Invalid("This field is required")
               | _ => Valid
               },
           },
         )
    );
};

module FormContainer = Formality.Make(Form);