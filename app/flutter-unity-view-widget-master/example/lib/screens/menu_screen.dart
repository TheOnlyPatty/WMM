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
      body: Container(
        decoration: BoxDecoration(
          image: DecorationImage(
            image: AssetImage("assets/images/tennis_stock.jpg"),
            fit: BoxFit.cover,
          ),
        ),
        child: Center(child: FlutterLogo(size: 250, colors: Colors.yellow)),
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
