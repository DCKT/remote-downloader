open BsLodash;

module Styles = {
  open Css;
  let root = style([background(hex("F7F8FB")), minHeight(pct(100.))]);
  let pageContent =
    style([
      maxWidth(px(550)),
      margin2(~v=px(0), ~h=auto),
      padding2(~v=px(40), ~h=px(0)),
    ]);
  let empty = style([fontSize(px(18)), textAlign(center)]);
};

[@react.component]
let make = () => {
  let (isModalVisible, setIsModalVisible) = React.useState(() => false);
  let (files, setFiles) = React.useState(() => [||]);

  React.useEffect1(
    () => {
      let db = Firebase.App.database(Firebase.app);

      Firebase.Database.on(
        Firebase.Database.ref(db, "files"),
        ~eventType="value",
        ~callback=snapshot => {
          let data = Firebase.DataSnapshot.val_(snapshot);
          let value = map(data);
          setFiles(_ => value);
          ();
        },
      )
      |> ignore;

      None;
    },
    [||],
  );

  <div className=Styles.root>
    <Navbar showModal={_event => setIsModalVisible(_ => true)} />
    <div className=Styles.pageContent>
      {
        Array.length(files) == 0 ?
          <p className=Styles.empty> {React.string("No downloads")} </p> :
          React.null
      }
      {
        files
        |> Array.mapi((index, file) =>
             <DownloadItem key={string_of_int(index)} file />
           )
        |> React.array
      }
    </div>
    <Modal
      isVisible=isModalVisible
      onClose={_ => setIsModalVisible(_ => false)}
    />
  </div>;
};