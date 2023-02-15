# Tutorial – Setup Bootstrap
 

## Introduction:
In this tutorial we are going to be familiarizing ourselves with the 2D drawing functionality of the AIE Bootstrap framework. Hopefully you will already be familiar with this framework, so for the most part this should be a bit of revision.

We won’t be concerning ourselves with any physics in this lecture. Instead, we will focus on drawing some 2D shapes. We’ll create a Breakout clone screen (not a game) by arranging some shapes in a 2D world. 

While our program won’t be interactive and will contain no game logic, you could extend this program throughout this course to create your own Breakout clone game.

## Downloading and Installing:
If you haven’t already done so, download the AIE Bootstrap framework from GitHub via this link:
https://github.com/AcademyOfInteractiveEntertainment/aieBootstrap/archive/master.zip 
This will directly download the framework for you to begin working with. 

Even if you already have a copy of this project, it is a good idea to download a fresh copy. This project is continually being updated, and the latest changes add support for Visual Studio 2022.

Alternatively, you may wish to set up your own Git project and clone the repository. Cloning the repository is not necessary, although it will allow you to be notified when updates and changes are made to the framework.

The landing page for this project contains some additional information, including video tutorials on how to set up a Git project: ```https://github.com/AcademyOfInteractiveEntertainment/aieBootstrap```

Once you have download the project, extract it to a folder on your machine. The project comes complete with a solution and a 2D and 3D demonstration project.

Before opening the solution, install the project templates if you have not already done so.

Locate the project templates in the aieBootstrap-master\tools\ProjectTemplates folder.

Copy the two .zip files into the Visual Studio templates directory. You’ll find this in your MyDocuments folder, for example:
	Documents\Visual Studio 2017\Templates\ProjectTemplates\Visual C++ Project\

Copy both of the .zip files to this location. Do not extract the .zip files.

After restarting Visual Studio you will now see two new project templates available whenever you create a new Visual C++ project.

## Clearing the Template Cache:
If you already have the templates installed and you wish to update them, you’ll need to clear the Visual Studio template cache.

The easiest way to do this is to use the Developer Command Prompt for Visual Studio 2022.

Before beginning, make sure Visual Studio is closed.

From the Windows Start menu, search for and open the Developer Command Prompt for VS 2022

This will launch a console window. Type the following command:
devenv /updateconfiguration

This will update the project template cache. Open Visual Studio again and you should now see both the 2D and 3D bootstrap project templates when you create a new project.

## Working with AIE Bootstrap:

The AIE Bootstrap framework comes with a provided Visual Studio solution (Bootstrap.sln). 

Open this solution in Visual Studio, or double-click on the .sln file.

The Bootstrap project is the framework itself. Every 2D or 3D project you make will link with this library to enable features like drawing using OpenGL, or handling keyboard input.

You will need to build the Bootstrap project. You can also build and execute the 2D and 3D projects if you wish to run these provided demonstration programs.

## Creating a New Project:

When working on a new tutorial, you will need to add your project to this solution.

Adding your project to this solution is much easier than creating a new solution for each tutorial and trying to import the Bootstrap library manually.

To create a new project, right-click on the solution name in the Solution Explorer and select Add -> New Project.

Select the 2D project template (since we will be creating our physics simulation in 2D).

In the Solution Explorer, right-click on your new project and select Set as StartUp Project. This means that when you press the Compile button, this is the project that will be compiled.

Finally, we need to set the execution path of the project.

Right-click on your new project and select Properties. 

Change the **Working Directory** under the **Debugging** settings to <b>```$(SolutionDir)bin\```</b>.

Then ensure the <b>Additional Library Directories</b> in <b>Linker</b> is set to <b>```$(SolutionDir)temp\Bootstrap\$(Platform)\$(Configuration)\;%(AdditionalLibraryDirectories)```</b>

And then ensure the <b>Additional Include Directories</b> in <b>C/C++</b> is set to <b>```$(SolutionDir)bootstrap;$(SolutionDir)dependencies/imgui;$(SolutionDir)dependencies/glm;%(AdditionalIncludeDirectories)```</b>

This will ensure the program can find all required libraries and resource files when it executes.

You’ve just created a new project that uses the AIEBootstrap library. 

## Retarget Your Project:
Before continuing with the rest of the tutorial, try to compile and run your project now.

If you get an error saying your project was made using a different version of the compiler, you’ll need to retarget the project. (This is because the templates have been made with a previous version of Visual Studio).

In the Solution Explorer, right-click on the project name and select Retarget Projects. Hit OK. 

Your project will then be set to use the latest SDK, and you can compile and execute as normal.

