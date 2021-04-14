import 'package:flutter_unity_widget_example/models/user.dart';
import 'package:flutter_unity_widget_example/screens/home/home.dart';
import 'package:flutter_unity_widget_example/screens/authenticate/authenticate.dart';
import "package:flutter/material.dart";
import 'package:flutter_unity_widget_example/screens/menu_screen.dart';
import 'package:provider/provider.dart';

class Wrapper extends StatelessWidget {
  @override
  Widget build(BuildContext context) {

    final user = Provider.of<User>(context);
    print(user);

    // return either Home or Authenticate widget
    if (user == null) {
      return Authenticate();
    } else {
      return Home();
    }
  }
}