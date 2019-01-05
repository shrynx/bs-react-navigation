open BsReactNavigation;

type route =
  | Home
  | UserDetails(string);


type navigationProp = NestedStackNavigator.navigation(route);

type loginRoute = 
  | Login
  | LoggedIn

type loginNavigation = SwitchNavigator.navigation(loginRoute);
