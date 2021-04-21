import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/models/athlete.dart';
import 'package:flutter_unity_widget_example/services/auth.dart';
import 'package:flutter_unity_widget_example/shared/nav_drawer.dart';
import 'package:flutter_unity_widget_example/utils/screen_utils.dart';
import 'package:provider/provider.dart';

class MenuScreen extends StatefulWidget {
  MenuScreen({Key key}) : super(key: key);

  @override
  _MenuScreenState createState() => _MenuScreenState();
}

class _MenuScreenState extends State<MenuScreen> {

  final AuthService _auth = AuthService();

  List<_MenuListItem> menus = [

    new _MenuListItem(
      description: 'Database',
      route: '/db',
      title: 'Database',
      enableAR: false,
    ),

    new _MenuListItem(
      description: 'Unity 3D Model of Past Data',
      route: '/un',
      title: '3D Model',
      enableAR: false,
    )
  ];

  @override
  Widget build(BuildContext context) {

    return Scaffold(
      backgroundColor: Color(0xff5d73a5),
      drawer: NavDrawer(),
      appBar: AppBar(
        title: Text('WMM Companion'),
        backgroundColor: Color(0xff0c1423),
        elevation: 0.0,
        actions: <Widget>[
          FlatButton.icon(
            icon: Icon(Icons.person),
            label: Text('logout'),
            textColor: Colors.white,
            onPressed: () async {
              await _auth.signOut();
            },
          )
        ]
      ),
      body: Center(
        child: ListView.builder(
          itemCount: menus.length,
          itemBuilder: (BuildContext context, int i) {
            return ListTile(
              title: Text(menus[i].title, style: TextStyle(color: Colors.white)),
              subtitle: Text(menus[i].description, style: TextStyle(color: Colors.white)),
              onTap: () {
                Navigator.of(context).pushNamed(
                  menus[i].route,
                  arguments: ScreenArguments(enableAR: menus[i].enableAR),
                );
              },
            );
          },
        ),
      ),
    );
  }
}

class _MenuListItem {
  final String title;
  final String description;
  final String route;
  final bool enableAR;

  _MenuListItem({this.title, this.description, this.route, this.enableAR});
}
