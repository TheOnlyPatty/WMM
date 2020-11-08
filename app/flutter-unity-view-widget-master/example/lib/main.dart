import 'package:flutter/material.dart';
import 'package:flutter_unity_widget_example/db.dart';

import 'screens/menu_screen.dart';
import 'screens/api_screen.dart';
import 'screens/loader_screen.dart';
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
    return MaterialApp(
      title: 'Flutter Unity Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      initialRoute: '/',
      routes: {
        '/': (context) => MenuScreen(),
        '/simple': (context) => SimpleScreen(),
        '/api': (context) => ApiScreen(),
        '/ble': (context) => BLEScreen(),
        '/db': (context) => DataBase(),
        '/un': (context) => UnityScreen(),
      },
    );
  }
}
