def : Function {
    let name = "olCreateKernel";
    let desc = "";
    let details = [];
    let params = [
        Param<"ol_program_handle_t", "Program", "handle of the program", PARAM_IN>,
        Param<"const char*", "KernelName", "name of the kernel entry point in the program", PARAM_IN>,
        Param<"ol_kernel_handle_t*", "Kernel", "output pointer for the created kernel", PARAM_OUT>
    ];
    let returns = [];
}

def : Function {
    let name = "olRetainKernel";
    let desc = "Create a queue for the given device";
    let details = [];
    let params = [
        Param<"ol_kernel_handle_t", "Kernel", "handle of the kernel", PARAM_IN>
    ];
    let returns = [];
}

def : Function {
    let name = "olReleaseKernel";
    let desc = "Create a queue for the given device";
    let details = [];
    let params = [
        Param<"ol_kernel_handle_t", "Kernel", "handle of the kernel", PARAM_IN>
    ];
    let returns = [];
}

def : Function {
    let name = "olSetKernelArgValue";
    let desc = "Create a queue for the given device";
    let details = [];
    let params = [
        Param<"ol_kernel_handle_t", "Kernel", "handle of the kernel", PARAM_IN>,
        Param<"uint32_t", "Index", "index of the argument", PARAM_IN>,
        Param<"size_t", "Size", "size of the argument data", PARAM_IN>,
        Param<"void*", "ArgData", "pointer to the argument data", PARAM_IN>
    ];
    let returns = [];
}

def : Function {
    let name = "olSetKernelArgsFromData";
    let desc = "Use a pre-defined array of argument data. Invalidates previous olSetKernelArg* calls.";
    let details = [
        "All kernel arguments are set at once.",
        "Previous calls to olSetKernelArgValue, olSetKernelArgsFromData on this kernel are invalidated.",
        "The implementation makes a copy of the data; ownership is not transferred."
    ];
    let params = [
        Param<"void*", "ArgData", "pointer to the argument data">,
        Param<"size_t", "ArgDataSize", "size of the data pointed to by ArgData">
        // TODO: Optional indices (figure out if they are used?)
    ];
    let returns = [];
}
