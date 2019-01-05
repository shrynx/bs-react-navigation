type navigation('a) = {
  push: 'a => unit,
  pop: 'a => unit,
};

[@bs.deriving abstract]
type screenOptions = {
  [@bs.optional]
  title: string,
};

module type StackConfig = {
  type route;
  let initialRoute: route;
  type extNav;

  let getScreen:
    (route, navigation(route), extNav) =>
    (ReasonReact.reactElement, screenOptions);
};

module Create = (Config: StackConfig) => {
  [@bs.deriving abstract]
  type navigatorConfig = {initialRouteName: string};

  [@bs.deriving abstract]
  type routeProps = {route: Config.route};

  module NavigationProp = {
    type t;

    module State = {
      type t;

      [@bs.get] external getParams: t => option(routeProps) = "params";
    };

    [@bs.send] external push: (t, string, routeProps) => unit = "push";

    [@bs.get "state"] external getState: t => State.t = "";

    let getParams = t => getState(t) |> State.getParams;
  };

  module ScreenOptions = {
    type t = {. "navigation": NavigationProp.t};
  };

  [@bs.deriving abstract]
  type routeConfig = {
    params: routeProps,
    screen: ScreenOptions.t => ReasonReact.reactElement,
    navigationOptions: ScreenOptions.t => screenOptions,
  };

  let containerDisplayName = "$bs-react-navigation_container";

  let makeNavigationProp = (navigation: NavigationProp.t) => {
    push: route =>
      NavigationProp.push(
        navigation,
        containerDisplayName,
        routeProps(~route),
      ),
    pop: _route => (),
  };

  let getCurrentScreen = (navigation: NavigationProp.t, externalNavigation) => {
    /** Params can be `null` in React Navigation, but we are always declaring them */
    let params = NavigationProp.getParams(navigation) |> Js.Option.getExn;
    let nav = makeNavigationProp(navigation);

    Config.getScreen(routeGet(params), nav, externalNavigation);
  };

  module Container = {
    let component = ReasonReact.statelessComponent("StackContainer");

    let make = (~navigation: NavigationProp.t, ~externalNavigation, _children) => {
      ...component,
      render: _self => getCurrentScreen(navigation, externalNavigation) |> fst,
    };
  };

  let route = (externalNavigation) => 
    routeConfig(
      ~params=routeProps(~route=Config.initialRoute),
      ~screen=
        (options: ScreenOptions.t) =>
          <Container navigation=options##navigation externalNavigation />,
      ~navigationOptions=
        (options: ScreenOptions.t) =>
          getCurrentScreen(options##navigation, externalNavigation) |> snd,
    );

  /* StackNavigator route */


  /* StackNavigator config */
  let config = navigatorConfig(~initialRouteName=containerDisplayName);

  let routes = Js.Dict.empty();

  /* Router */
  let router = externalNavigation => ReactNavigation.Core.stackRouter({
    
    Js.Dict.set(routes, containerDisplayName, route(externalNavigation));
    routes
  }, config);

  /* navigator */
  let navigator = externalNavigation => 
    ReactNavigation.Core.createNavigator(
      ReactNavigation.Stack.stackView,
      router(externalNavigation),
      config,
    );

  /* Wrap StackNavigator with the AppContainer - temporary */
  let render = externalNavigation => ReactNavigation.Native.createAppContainer(navigator(externalNavigation));

  let make = ( ~externalNavigation, _children) => {
    ...ReasonReact.statelessComponent("NestedStackContainer"),
    render: _self => ReasonReact.createElement(render(externalNavigation), [||])
  };
};