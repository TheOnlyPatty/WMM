import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/db.dart';
import 'package:flutter_unity_widget_example/models/user.dart';
import 'package:flutter_unity_widget_example/screens/wrapper.dart';
import 'package:flutter_unity_widget_example/services/auth.dart';
import 'package:provider/provider.dart';

import 'screens/menu_screen.dart';
import 'screens/api_screen.dart';
import 'screens/simple_screen.dart';
import 'ble_screen.dart';
import 'unity_screen.dart';


void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return StreamProvider<User>.value(
      value: AuthService().user,
      child: MaterialApp(
        home: Wrapper(), 
      ),
    );
  }
}
