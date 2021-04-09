import "package:flutter/material.dart";
import 'package:flutter_unity_widget_example/ble_screen.dart';
import 'package:flutter_unity_widget_example/db.dart';
import 'package:flutter_unity_widget_example/screens/api_screen.dart';
import 'package:flutter_unity_widget_example/screens/menu_screen.dart';
import 'package:flutter_unity_widget_example/screens/simple_screen.dart';
import 'package:flutter_unity_widget_example/unity_screen.dart';

class Home extends StatelessWidget {

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