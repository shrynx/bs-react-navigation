open BsReactNative;

module Styles = {
  open Style;

  let container =
    style([
      marginTop(Pt(50.0)),
      alignItems(Center),
      justifyContent(Center),
    ]);
};

let component = ReasonReact.statelessComponent("UserDetail");

let make =
    (~navigation: Config.navigationProp, 
    ~loginNavigation: Config.loginNavigation,
     ~text: string="Hi!", _children) => {
  ...component,
  render: _self =>
    <SafeAreaView>
      <View style=Styles.container>
        <Text> {ReasonReact.string(text)} </Text>
        <Button
          title="Back to home"
          onPress={() => navigation.push(Home)}
        />
        <Button 
          title="Log out"
          onPress={() => loginNavigation.navigate(Login)}
        />
      </View>
    </SafeAreaView>,
};