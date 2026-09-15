export default function About() {
  return (
    <main className="flex-1 bg-zinc-950 text-white px-4 sm:px-8 py-16">
      <section className="max-w-3xl mx-auto">

        <h1 className="text-4xl sm:text-5xl font-bold tracking-tight text-center">
          About Maryn Launcher
        </h1>
        <p className="mt-6 text-zinc-400 text-lg text-center">
          Maryn Launcher is a lightweight, open-source Minecraft Java Edition launcher built with C++ and Qt6.
        </p>

        <div className="mt-12 space-y-6">
          <div className="bg-zinc-900 border border-zinc-800 rounded-xl p-6">
            <h2 className="text-xl font-semibold mb-2">Why Maryn?</h2>
            <p className="text-zinc-400 text-sm">
              Most Minecraft launchers are built on Electron — which bundles an entire browser engine just to run a desktop app. Maryn is built natively with C++ and Qt6, keeping it fast and lightweight.
            </p>
          </div>

          <div className="bg-zinc-900 border border-zinc-800 rounded-xl p-6">
            <h2 className="text-xl font-semibold mb-2">Current Features</h2>
            <ul className="text-zinc-400 text-sm space-y-1 list-disc list-inside">
              <li>Offline / cracked account support</li>
              <li>Native C++ and Qt6 — no Electron</li>
              <li>Available for Windows, Linux, and macOS</li>
            </ul>
          </div>

          <div className="bg-zinc-900 border border-zinc-800 rounded-xl p-6">
            <h2 className="text-xl font-semibold mb-2">Community</h2>
            <p className="text-zinc-400 text-sm mb-4">
              Join the community to get help, share feedback, or follow development.
            </p>
            <div className="flex gap-4">
              
               <a href="#"
                className="bg-indigo-600 hover:bg-indigo-500 text-white px-4 py-2 rounded-lg text-sm font-semibold transition-colors"
              >
                Discord (coming soon)
              </a>
              
                <a href="https://github.com/DinankaKoirala/Maryn_Launcher"
                target="_blank"
                rel="noopener noreferrer"
                className="bg-zinc-700 hover:bg-zinc-600 text-white px-4 py-2 rounded-lg text-sm font-semibold transition-colors"
              >
                GitHub
              </a>
            </div>
          </div>
        </div>

      </section>
    </main>
  )
}