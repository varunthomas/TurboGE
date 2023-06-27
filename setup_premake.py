import requests
import zipfile
import io
import os

class PremakeConfiguration:
    premakeVersion = "5.0.0-beta2"
    premakeZipUrls = f"https://github.com/premake/premake-core/releases/download/v{premakeVersion}/premake-{premakeVersion}-windows.zip"
    premakeLicenseUrl = "https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt"
    premakeDirectory = "./vendor/bin/premake"

    @classmethod
    def DownloadPremake(self):
        #premakePath = f"{self.premakeDirectory}/premake-{self.premakeVersion}-windows.zip"
        premakePath = self.premakeDirectory
        filePath = os.path.abspath(premakePath)
        os.makedirs(os.path.dirname(filePath), exist_ok=True)
        
        print("Downloading premake...")
        response = requests.get(self.premakeZipUrls)
    
        # Check if the request was successful
        if response.status_code == 200:
            # Read the content of the zip file
            zip_content = io.BytesIO(response.content)
            
            # Extract the contents of the zip file
            print("Extracting to folder " + filePath)
            with zipfile.ZipFile(zip_content, 'r') as zip_ref:
                # Extract all files to the destination path
                zip_ref.extractall(filePath)
            
            print("Zip file downloaded and extracted successfully.")
            
            
            #Keep only premake5.exe
            file_to_keep = "premake5.exe"
            files = os.listdir(premakePath)

            # Iterate over the files and delete them except for the desired file
            for file in files:
                file_path = os.path.join(filePath, file)
                if file != file_to_keep:
                    os.remove(file_path)
        else:
            print("Failed to download the zip file.")
    

PremakeConfiguration.DownloadPremake()