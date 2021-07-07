import winreg

class Regedit:
    def __setitem__(self, name, value):
        name = name.replace("/", "\\")
        items = name.split("\\")
        root_dir = items[0].upper()
        key = items[-1]
        path = '\\'.join(items[1:-1])

        winreg.CreateKey(eval("winreg." + root_dir), path)

        registry_key = winreg.OpenKey(eval("winreg." + root_dir), path, 0, winreg.KEY_ALL_ACCESS)
        old_value, regtype = winreg.QueryValueEx(registry_key, key)
        if str(value) == old_value:
            return

        winreg.SetValueEx(registry_key, key, 0, regtype, str(value))
        winreg.CloseKey(registry_key)
        
    def __getitem__(self, name):
        name = name.replace("/", "\\")
        items = name.split("\\")
        root_dir = items[0].upper()
        key = items[-1]
        path = '\\'.join(items[1:-1])

        registry_key = winreg.OpenKey(eval("winreg." + root_dir), path, 0, winreg.KEY_READ)
        value, regtype = winreg.QueryValueEx(registry_key, key)
        winreg.CloseKey(registry_key)
        return value

regedit = Regedit()
