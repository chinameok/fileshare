
var sys = require('sys');
var gui=require('nw.gui');
var exec = require('child_process').exec;

var net = require('net');
var port = 8800;

//var iconv = require('iconv-lite');
var userList;
var groupList;
var submitRes = {};
var cancelRes;
var conn;
function submit()
{
	var directory = $('#inputName').val();
	var users = {};
	var groups = {};
	$('#userTable tr').each(function() {
		if($(this).find('td').eq(0).find('p').text() == "")
			return true;
		users[$(this).find('td').eq(0).find('p').text()] = $(this).find('td').eq(1).find('option:selected').val();
		console.log($(this).find('td').eq(0).find('p').text());
	});
	$('#groupTable tr').each(function() {
		if($(this).find('td').eq(0).find('p').text() == "")
			return true;
		groups[$(this).find('td').eq(0).find('p').text()] = $(this).find('td').eq(1).find('option:selected').val();
	});

	submitRes['dir'] = gui.App.argv[0];
	submitRes['users'] = users;
	submitRes['groups'] = groups;
	submitRes ['disname'] = directory;
	var strSubmitRes = JSON.stringify(submitRes);
	conn = net.createConnection(port);

	conn.on('connect',function(){
		console.log('connected to server');
		var retVal = conn.write(strSubmitRes);
		//var retVal = conn.write('Puck you!');
		console.log("send buf to server:"+retVal);
	});
	conn.on('error', function(err) {
		console.error('Error in connection:' + err.message + ', code: ' + err.code);
	});
	conn.on('close',function(err){
		console.log('connection closed!');
	});
/*	var express =  'shareconfig ' + '\'' + strSubmitRes + '\'';
	console.log(express);
	exec(express, { encoding: 'utf8',
			timeout: 100000,
			maxBuffer: 200*1024,
			killSignal: 'SIGTERM',
			cwd: null,
			env: null },
			function(error, stdout, stderr)
			{
				if(stderr) {
					//console.log(iconv.decode(stderr,'utf8'));
					console.log(stderr);
				}
				else if(error)
				{
					console.log(error);
				}
				else
				{
					console.log(stdout);
					return ;
				}
	});
	console.log("*****************8", strSubmitRes);*/
}

function cancel()
{
	process.exit();
}

var getDomainUserList = function(){
	var expr = "ldap user";

	exec(expr, function(error, stdout, stderr){
		if(stderr) {
			alert("ldap error");
			console.log(error, stderr);
			return;
		}
		userList = JSON.parse(stdout);
		var domainUserList = document.getElementById("username");

		$('#username').bind('focus', function(event){
		event.preventDefault();
		domainUserList.options.length = 0;
		for (var user in userList){
			domainUserList.options.add(new Option(userList[user], "username"));
		}
		});
		
		$('#username').bind('blur', function(event){
			event.preventDefault();
			$('#username').unbind('focus');
		});
	});
}

var getDomainGroupList = function(){
	var expr = "ldap group";

	exec(expr, function(error, stdout, stderr){
		if(stderr) {
			alert("ldap error");
			console.log(error, stderr);
			return ;
		}
		groupList = JSON.parse(stdout);
		var domainGroupList = document.getElementById("groupname");

		$('#groupname').bind('focus', function(event){
		event.preventDefault();
		domainGroupList.options.length = 0;
		for (var group in groupList){
			domainGroupList.options.add(new Option(groupList[group], "groupname"));
		}
		});
		
		$('#groupname').bind('blur', function(event){
			event.preventDefault();
			$('#groupname').unbind('focus');
		});
	});
}

function deleteTable(obj){
	$(obj) .parents("tr").remove();
}

var addUser =  function(event){
		event.preventDefault();
		if($('#username').val() == null) {
			alert("null user selected!");
			return;
		}
		var tableItem = $('#username').find("option:selected").text();
		var hasAdded = 0;
		$('#userTable tr').each(function() {
			if (tableItem == $(this).find('td').eq(0).find('p').text()) {
				alert("added");
				hasAdded = 1;
			}
		});
		if(hasAdded){
			return;
		}
		var rowNum = document.getElementById("userTable").rows.length;
		var newTr = document.getElementById("userTable").insertRow(rowNum );

		var newTd0 = newTr.insertCell();
		var newTd1 = newTr.insertCell();
		var newTd2 = newTr.insertCell();

		newTd2.innerHTML =  '<div style="height:30px; width:80px; overflow:hidden;"><p style="margin-top:5px;" title="' + tableItem +'">' + tableItem + '</p></div>';
		
		newTd1.innerHTML = '<div class="text-center" style="height:20px; padding:0px;"><select id="userprio" style="width:100px;"> <option value=0>可读</option><option value=1>可读写</option></select> </div>';
		newTd0 .innerHTML = "<div class=\"text-center\" style=\"height:20px; padding:0px;\"><button type=\"button\" class=\"btn btn-danger\" id=" + tableItem + " onclick=\"deleteTable(this)\">delete</button> </div>";
}
var addGroup =  function(event){
		event.preventDefault();
		if($('#groupname').val() == null) {
			alert("null user selected!");
			return;
		}
		var tableItem = $('#groupname').find("option:selected").text();
		var hasAdded = 0;
		$('#groupTable tr').each(function() {
			if (tableItem == $(this).find('td').eq(0).find('p').text()) {
				alert("added");
				hasAdded = 1;
			}
		});
		if(hasAdded){
			return;
		}
		var rowNum = document.getElementById("groupTable").rows.length;
		var newTr = document.getElementById("groupTable").insertRow(rowNum );

		var newTd0 = newTr.insertCell();
		var newTd1 = newTr.insertCell();
		var newTd2 = newTr.insertCell();

		newTd2.innerHTML =  '<div style="height:30px; width:80px; overflow:hidden;"> <p style="margin-top:5px;" title="' +tableItem +'">' + tableItem + '</p></div>';
		
		newTd1.innerHTML = '<div class="text-center" style="height:20px; padding:0px;"><select id="groupprio" style="width:100px;"><option value=0>可读</option> <option value=1>可读写</option></select> </div>';
		newTd0 .innerHTML = "<div class=\"text-center\" style=\"height:20px; padding:0px;\"><button type=\"button\" class=\"btn btn-danger\" id=" + tableItem + " onclick=\"deleteTable(this)\">delete</button> </div>";
}

$(document).ready(function(){
	document.getElementById('filepath').innerHTML=  decodeURI(gui.App.argv[0]);
	getDomainUserList();
	getDomainGroupList();

	$('#addUser').bind('click', addUser);
	$('#addGroup').bind('click', addGroup);
	$('#submit').bind('click', submit);
	$('#cancel').bind('click', cancel);
});
