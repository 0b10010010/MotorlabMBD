classdef Count2Angle < matlab.System ...
        & coder.ExternalDependency ...
        & matlab.system.mixin.Propagates ...
        & matlab.system.mixin.CustomIcon
    %
    % System object template for a sink block.
    % 
    % This template includes most, but not all, possible properties,
    % attributes, and methods that you can implement for a System object in
    % Simulink.
    %
    % NOTE: When renaming the class name Sink, the file name and
    % constructor name must be updated to use the class name.
    %
    
    % Copyright 2014 The MathWorks, Inc.
    %#codegen
    %#ok<*EMCA>
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.
    end
    
    properties (Access = private)
        % Pre-computed constants.
    end
    
    methods
        % Constructor
        function obj = Count2Angle(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
    end
    
    methods (Access=protected)
        function setupImpl(obj) %#ok<MANU>
            if coder.target('Rtw')
                % Call C-function implementing device initialization
                coder.cinclude('Count2Angle.h');
            else
                % Place simulation setup code here
            end
        end
        
        function y = stepImpl(obj,u)  %#ok<INUSD>
            y = double(0);
            if coder.target('Rtw')
                % Call C-function implementing device output
                y = coder.ceval('Count2Angle',u);
            else
                % Place simulation output code here
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if coder.target('Rtw')
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');
            else
                % Place simulation termination code here
            end
        end
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 1;
        end
        
        function num = getNumOutputsImpl(~)
            num = 1;
        end
        
        function flag = isInputSizeLockedImpl(~,~)
            flag = true;
        end
        
        function varargout = isInputFixedSizeImpl(~,~)
            varargout{1} = true;
        end
        
        function flag = isInputComplexityLockedImpl(~,~)
            flag = true;
        end
        
        function varargout = isInputComplexImpl(~)
            varargout{1} = false;
        end
        
        function varargout = getOutputDataTypeImpl(~)
            varargout{1} = 'double';
        end
        
        function validateInputsImpl(~, u)
            if isempty(coder.target)
                % Run input validation only in Simulation
                validateattributes(u,{'uint32'},{'scalar'},'','u');
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = 'Count2Angle';
        end
    end
    
    methods (Static, Access=protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'Count2Angle';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                % Update buildInfo
                srcDir = fullfile(fileparts(mfilename('fullpath')),'source');
                incDir = fullfile(fileparts(mfilename('fullpath')),'source','include');
                buildInfo.addIncludePaths(incDir);
                % Use the following API's to add include files, sources and
                % linker flags
                addIncludeFiles(buildInfo,'Count2Angle.h');         
                addSourceFiles(buildInfo,'Count2Angle.c',srcDir);
                %addLinkFlags(buildInfo,{'-llibSink'});
            end
        end
    end
end
