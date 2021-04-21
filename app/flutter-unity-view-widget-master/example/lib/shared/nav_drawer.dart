import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/utils/screen_utils.dart';

class NavDrawer extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Drawer(
      child: ListView(
        padding: EdgeInsets.zero,
        children: <Widget>[
          DrawerHeader(
            child: Text(
              '',
              style: TextStyle(color: Colors.white, fontSize: 25),
            ),
            decoration: BoxDecoration(
              color: Color(0xff0c1423),
              image: DecorationImage(
                alignment: Alignment.center,
                fit: BoxFit.scaleDown,
                image: AssetImage('assets/images/WMM_whiteLogo.png')
              ),
            ),
          ),
          ListTile(
            leading: Icon(Icons.input, color: Color(0xff0c1423)),
            title: Text('Home'),
            onTap: () => {Navigator.of(context).pop()},
          ),
          ListTile(
            leading: Icon(Icons.bluetooth, color: Color(0xff0c1423)),
            title: Text('Bluetooth Setup'),
            onTap: () => {
              Navigator.of(context).pushNamed('/ble', arguments: ScreenArguments(enableAR: false),)
            },
          ),
          ListTile(
            leading: Icon(Icons.account_circle, color: Color(0xff0c1423)),
            title: Text('Profile'),
            onTap: () => {
              Navigator.of(context).pushNamed('/al', arguments: ScreenArguments(enableAR: false),)
            },
          ),
          ListTile(
            leading: Icon(Icons.play_arrow, color: Color(0xff0c1423) ),
            title: Text('Record new data'),
            onTap: () => {
              Navigator.of(context).pushNamed('/ble', arguments: ScreenArguments(enableAR: false),)
            },
          ),
          // ListTile(
          //   leading: Icon(Icons.exit_to_app),
          //   title: Text('Logout'),
          //   onTap: () => {
              
          //   },
          // ),
        ],
      ),
    );
  }
}