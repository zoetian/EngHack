(function(){


	'use strict';

	angular
		.module('app', ['ngRoute', 'ngCookies'])
		.config(config)
		.run(run);

		config.$inject = ['$routeProvider', '$locationProvider'];
		function config ($routeProvider, $locationProvider){
			$routeProvider
			.when('/',{

				controller:'Search',
				templateUrl: 'search.html',
				controllerAs:'vm'
			})

			.when('/login',{

				controller:'Login',
				templateUrl: 'login.html',
				controllerAs:'vm'
			})

			.otherwise({ridirectTo: '/login'})

		}
}