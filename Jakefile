var isAsync = { async: true };
Array.prototype.toPath = function() {
	var array = this.slice(0);

	for (var i = array.length; i--; ) {
		if (array[i] === undefined)
			array.splice(i, 1);
	}

	return array.join('/');
};
Array.prototype.toMessage = function() {
	return this.join('');
};
Array.prototype.toCommand = function() {
	return this.join(' ');
};

var chalk = require('chalk');
var cc = process.env.cc || 'g++';
var cflags = process.env.cflags || '-std=c++11';
var options = process.env.options || '-Wall';
var libs = process.env.libs || '-lm';
var defines = process.env.defines || '';

var sourceDirectory = 'src';
var outputDirectory = 'bin';
var objectDirectory = 'obj';
var includeDirectory = 'include';

var targetDirectories = {
	debug: 'debug',
	release: 'release'
};

var applicationName = 'harmonic';

var files = new jake.FileList();
	files.include(sourceDirectory + '/*.cpp');

var targets = {
	debug: [outputDirectory, targetDirectories.debug, applicationName].toPath(),
	release: [outputDirectory, targetDirectories.release, applicationName].toPath()
};

var info = function(sender, message) {
	jake.logger.log(['[', chalk.green(sender), '] ', chalk.gray(message)].toMessage());
};
var warn = function(sender, message) {
	jake.logger.log(['[', chalk.red(sender), '] ', chalk.gray(message)].toMessage());
};
var targetFileNames = function(targetDirectory) {
	return function(fileName) {
		var subDirectory = [objectDirectory, targetDirectory].toPath();
		return fileName.replace(sourceDirectory, subDirectory).replace('.cpp', '.o');
	};
};
var sourceFileName = function(fileName) {
	var index = fileName.lastIndexOf('/');
	return sourceDirectory + fileName.substr(index).replace('.o', '.cpp');
};
var link = function(target, objs, compiler, flags, callback) {
	var sources = objs.toCommand();
	var cmd = [compiler, flags, '-o', target, sources, options, libs].toCommand();
	info(compiler, 'Linking ' + chalk.magenta(target) + ' ...');
	jake.exec(cmd, callback);
};
var strip = function(name, callback) {
	var cmd = ['strip', name].toCommand();
	info('strip', 'Removing symbols from ' + chalk.magenta(name) + ' ...');
	jake.exec(cmd, callback);
};
var compile = function(name, source, compiler, flags, optimizationLevel, callback) {
	var includes = ['-c', '-I', includeDirectory].toCommand();
	var locals = [defines].toCommand();
	var cmd = [compiler, flags, includes, '-o', name, source, options, optimizationLevel, locals].toCommand();
	info(compiler, 'Compiling ' + chalk.magenta(source) + ' ...');
	jake.exec(cmd, callback);
};
var remove = function(name) {
	var cmd = ['rm', '-rf', name].toCommand();
	jake.exec(cmd);
};
var ruleCreator = function(r) {
	var condition = new RegExp('/' + r.source + '/.+' + '\\.o$');
	var destination = r.target.substr(0, r.target.lastIndexOf('/'));

	rule(condition, sourceFileName, isAsync, function() {
		jake.mkdirP([objectDirectory, r.source].toPath());
		var name = this.name;
		var source = this.source;
		compile(name, source, r.compiler, r.flags, r.optimization, function() {
			info(r.compiler, 'Compiled ' + chalk.magenta(source) + ' to ' + chalk.magenta(name) + '.');
			complete();
		});
	});

	file(r.target, r.objects, isAsync, function() {
		jake.mkdirP(destination);
		link(r.target, r.objects, r.compiler, r.flags, function() {
			info(r.compiler, 'Linked ' + chalk.magenta(r.target) + '.');
			complete();
		});
	});
};
var rules = [{
	source: [targetDirectories.debug].toPath(),
	optimization: '-O0 -g -DDEBUG',
	flags: cflags,
	compiler: cc,
	target: targets.debug,
	objects: files.toArray().map(targetFileNames(targetDirectories.debug))
},{
	source: [targetDirectories.release].toPath(), 
	optimization: '-O2', 
	flags: cflags,
	compiler: cc,
	target: targets.release, 
	objects: files.toArray().map(targetFileNames(targetDirectories.release))
}];

rules.forEach(ruleCreator);

desc('Removes the previously created object files');
task('clean', function(params) {
	info('clean', 'Cleaning up everything ...');

	rules.forEach(function(r) {
		var path = [objectDirectory, r.source, '*.o'].toPath();
		remove(path);
		info('clean', 'Removed all files matching ' + chalk.magenta(path) + '.');
	});

	info('clean', 'Everything done!');
});

desc('Creates a debug version of the application');
task('debug', [targets.debug], function(params) {
	info('debug', 'Everything done!');
});

desc('Creates a release version of the application');
task('release', [targets.release], isAsync, function(params) {
	strip(targets.release, function() {
		info('release', 'Removed symbols from ' + chalk.magenta(targets.release) + '.');
		info('release', 'Everything done!');
		complete();
	});
});

desc('Creates all versions of the application');
task('default', ['debug', 'release'], function(params) {
	info('default', 'Everything done!');
});