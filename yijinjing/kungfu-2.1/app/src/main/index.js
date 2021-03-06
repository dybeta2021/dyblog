
// Modules to control application life and create native browser window
const path = require('path');

if (process.env.NODE_ENV !== 'development') {
    global.__resources = path.join(__dirname, '/resources').replace(/\\/g, '\\\\')
}

const { app, BrowserWindow, Menu, dialog, ipcMain } = require('electron');
const electron = require('electron');
//base setting, init db
const { initDB, initConfig } = require('./base');
const { killGodDaemon,  killExtra, killKfc, killKungfu } = require('__gUtils/processUtils');
const { logger } = require('__gUtils/logUtils');
const { platform } = require('__gConfig/platformConfig');
const packageJSON = require('__root/package.json');
const { KF_HOME, KUNGFU_ENGINE_PATH } = require('__gConfig/pathConfig');


//create db
initDB();
initConfig();
setMenu();


// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
var mainWindow = null;
var allowQuit = false;
function createWindow () {
	// Create the browser window.
	const electronScreen = electron.screen;    
	const { width,height } = electronScreen.getPrimaryDisplay().size
	mainWindow = new BrowserWindow({
		show: false,
		width,
		height,
		useContentSize: true,
		webPreferences: {
			nodeIntegration: true
		},
		backgroundColor: '#161B2E',
	})
	const isDevelopment = process.env.NODE_ENV === "development" 
	// and load the index.html of the app.
	if(isDevelopment){
		mainWindow.loadURL('http://localhost:9090')
	}else{
		const filePath = path.join(__dirname, "index.html");
		mainWindow.loadFile(filePath)
	}

	// Open the DevTools.
	// mainWindow.webContents.openDevTools()

	// // Emitted when the window is closed.
	mainWindow.on('close', (e) => {
	// Dereference the window object, usually you would store windows
	// in an array if your app supports multi windows, this is the time
	// when you should delete the corresponding element.
		if (platform !== 'mac' && !allowQuit) {
			showQuitMessageBox();	
			e.preventDefault();
		}
		else return
	})

	mainWindow.on('crashed', () => {
		logger.error('crashed', new Date())
		mainWindow && mainWindow.reload()
	})

	mainWindow.on('unresponsive', () => {
		logger.error('unresponsive', new Date())
		mainWindow && mainWindow.reload()
	})

	mainWindow.on('ready-to-show', function() {
		mainWindow.show();
		mainWindow.focus();
	});
}


//??????????????????
const gotTheLock = app.requestSingleInstanceLock()
if(!gotTheLock) {
	allowQuit = true;
	app.quit()
} else {
	app.on('second-instance', (event, commandLine, workingDirectory) => {
		if(mainWindow) {
			if(mainWindow.isMinimized()) mainWindow.restore()
			mainWindow.focus()
		}
	})
}




// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
var appReady = false, killExtraFinished = false;
app.on('ready', () => {
	appReady = true;
	if(appReady && killExtraFinished) createWindow()
})

//????????????????????????????????????????????? pm2/kfc ??????kill???
killExtra()
.catch(err => logger.error(err))
.finally(() => {
	killExtraFinished = true;
	if(appReady && killExtraFinished) createWindow()
})

// Quit when all windows are closed.
app.on('window-all-closed', function (e) {
// On macOS it is common for applications and their menu bar
// to stay active until the user quits explicitly with Cmd + Q
	 if (platform !== 'mac') app.quit()
})

app.on('activate', function () {
// On macOS it's common to re-create a window in the app when the
// dock icon is clicked and there are no other windows open.
????????if (mainWindow.isDestroyed()) createWindow()
????????else if(mainWindow.isVisible()) mainWindow.focus()
????????else mainWindow.show()
})

app.on('will-quit', async (e) => {
	if(allowQuit) return
	if(process.env.APP_TYPE === 'test') {
		try {
			await KillAll()
		} catch (err) {
			console.error(err)
		}
		app.quit()
		return;
	}
	if (platform === 'mac') showQuitMessageBox();
	e.preventDefault()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.

function setMenu() {
    //???????????????
	let applicationOptions = [
		{ label: "About Kungfu", click: showKungfuInfo},
		{ label: "Settings", accelerator: "CmdOrCtrl+,", click: openSettingDialog },
		{ label: "Close", accelerator: "CmdOrCtrl+W", click: function() { console.log(BrowserWindow.getFocusedWindow().close()); }}
	]

	if(platform === 'mac') {
		applicationOptions.push(
			{ label: "Quit", accelerator: "Command+Q", click: function() { app.quit(); }},
		)
	}

	const template = [
	{
		label: "Kungfu",
		submenu: applicationOptions
	}, 
	{
		label: "Edit",
		submenu: [
			{ label: "Copy", accelerator: "CmdOrCtrl+C", selector: "copy:" },
			{ label: "Paste", accelerator: "CmdOrCtrl+V", selector: "paste:" },
		]
	}];
	
	Menu.setApplicationMenu(Menu.buildFromTemplate(template))
}

function showKungfuInfo() {
	const version = packageJSON.version;
	const electronVersion = packageJSON.devDependencies.electron;
	const info = `Version: ${version}\n`
	+ `electron: ${electronVersion} \n`
	+ `platform: ${platform} \n`
	+ `kfHome: ${KF_HOME} \n`
	+ `kungfuEngine: ${path.resolve(KUNGFU_ENGINE_PATH, 'kfc')} \n`
	+ `commit: ${git_commit_version}`
	dialog.showMessageBox({
		type: 'info',
		message: 'Kungfu',
		defaultId: 0,
		detail: info,
		buttons: ['??????'],
		icon: path.join(__resources, 'icon', 'icon.png')
	})
}

//????????????renderprocess ??????????????????
function openSettingDialog() {
	mainWindow.webContents.send('main-process-messages', 'open-setting-dialog')
	mainWindow.focus()
}

//????????????
function showQuitMessageBox(){
	dialog.showMessageBox({
		type: 'question',
		title: '??????',
		defaultId: 0,
		cancelId: 1,
		message: "????????????????????????????????????????????????????????????",
		buttons: ['??????', `????????????${platform !== 'mac' ? '?????????' : ' Dock'}`],
		icon: path.join(__resources, 'icon', 'icon.png')
	}, (index) => {
		if(index === 0){
			KillAll().then(() => app.quit())
		}else{
			if((mainWindow !== null) && !mainWindow.isDestroyed()){
				if(platform === 'mac') mainWindow.hide();
				else mainWindow.minimize();
			}
		}
	})
}

//??????????????????
function KillAll(){
	return new Promise(resolve => {
		if(mainWindow && !mainWindow.isDestroyed()) mainWindow.hide()
		allowQuit = true;
		console.time('kill kfcs');
		killKfc()
		.catch(err => console.error(err)) 
		.finally(() => {
			console.timeEnd('kill kfcs');
			if(platform === 'linux') killKungfu()
			console.time('kill daemon');
			killGodDaemon()
			.catch(err => console.error(err)) 				
			.finally(() => {
				console.timeEnd('kill daemon');
				console.time('kill extra');
				killExtra()
				.catch(err => console.error(err)) 								
				.finally(() => {
					console.timeEnd('kill extra');	
					resolve(true)
				})
			})
		})
	})
}
