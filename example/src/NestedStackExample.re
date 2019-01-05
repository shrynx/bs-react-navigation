open BsReactNavigation;

/**
  * Route and Navigation types are kept in a separate module to
  * handle circular references between modules
  */
open Config;

module NestedStack =
  NestedStackNavigator.Create({
    open NestedStackNavigator;

    type route = Config.route;

    let initialRoute = Home;
    type extNav = Config.loginNavigation;

    let getScreen = (route, navigation, externalNavigation) =>
      switch (route) {
      | UserDetails(userId) => (
          <UserDetail
            navigation
            text={"Browsing profile of: " ++ userId}
            loginNavigation=externalNavigation
          />,
          screenOptions(~title="Hello " ++ userId, ()),
        )
      | Home => (
          <Home navigation loginNavigation=externalNavigation />,
          screenOptions(~title="Home", ()),
        )
      };
  });

let make = NestedStack.make;